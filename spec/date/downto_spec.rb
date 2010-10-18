require File.expand_path('../../spec_helper', __FILE__)

describe "Date#downto" do

  it "should be able to step backward in time" do
    ds    = Date.civil(2000, 4, 14)
    de    = Date.civil(2000, 3, 29)
    count = 0
    ds.downto(de) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end
    count.should == 17
  end

  it "should keep the same class as the receiver" do
    c = Class.new(Date)
    c.jd.downto(c.jd - 2) do |d|
      d.should be_kind_of(c)
    end.should be_kind_of(c)
  end
end
