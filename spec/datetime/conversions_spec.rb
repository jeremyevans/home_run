require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime conversions" do
  it "#new_offset should be a separate datetime with a modified offset" do
    DateTime.new(2008, 1, 1).new_offset(0.5).should == DateTime.new(2008, 1, 1, 12, 0, 0, 0.5)
  end

  ruby_version_is "" ... "1.9" do
    it "#newof should be a separate datetime with a modified offset" do
      DateTime.new(2008, 1, 1).newof(0.5).should == DateTime.new(2008, 1, 1, 12, 0, 0, 0.5)
    end
  end
end

describe "DateTime marshalling" do
  it "should marshal and unmarshal correctly" do
    Marshal.load(Marshal.dump(DateTime.jd)).should == DateTime.civil
    d = DateTime.now
    Marshal.load(Marshal.dump(d)).should == d
    Marshal.load(Marshal.dump(DateTime.civil(2010, 2, 4, 1, 2, 4, 4/24.0))).should == DateTime.civil(2010, 2, 4, 1, 2, 4, 4/24.0)
  end
end

