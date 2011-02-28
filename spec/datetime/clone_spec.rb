require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#clone" do
  before do
    @d = DateTime.now
  end

  it "should return a copy of the date" do
    @d.clone.should == @d 
  end

  it "should return a different object_id" do
    @d.clone.object_id.should_not == @d.object_id
  end

  it "should keep frozen status" do
    @d.freeze
    @d.clone.frozen?.should be_true
  end

  it "should keep singleton_methods" do
    class << @d; def foo() 1 end end
    @d.clone.respond_to?(:foo).should be_true
  end
end
